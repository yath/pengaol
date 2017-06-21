#ifdef WIN32
#error "This example is not supported on Windows!"
#endif // WIN32

#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>
#include <iostream.h>
#include <iomanip.h>
#include <qpthr/qp.h>

/*
 * 
 * Linux does not support resource counting accumulated from threads
 * so some test will report wrong cpu usage 
 *
 */
#ifdef ZERO_RUSAGE
int getrusage(int who, struct rusage *rusage)
{
	memset(rusage, 0, sizeof(struct rusage));
	return 0;
}
#endif

struct timeval timeval_sub(struct timeval *tv1, struct timeval *tv2)
{
	struct timeval retval;
		
	retval.tv_sec = tv1->tv_sec - tv2->tv_sec;
	if (tv1->tv_usec >= tv2->tv_usec) {
		retval.tv_usec = tv1->tv_usec - tv2->tv_usec;
	} else {
		retval.tv_sec--;
		retval.tv_usec = (tv1->tv_usec + 1000000) - tv2->tv_usec;
	}
	return retval;	
}

struct timeval timeval_add(struct timeval *tv1, struct timeval *tv2)
{
	struct timeval retval;
	
	retval.tv_sec = tv1->tv_sec + tv2->tv_sec;
	retval.tv_usec = tv1->tv_usec + tv2->tv_usec;
	retval.tv_sec += retval.tv_usec / 1000000;
	retval.tv_usec %= 1000000;
	return retval;
}

ostream& operator << (ostream& o, struct timeval& tv)
{
	o.fill('0');
	return o << tv.tv_sec << "." << setw(3) << tv.tv_usec / 1000;
}

class TimeMeasure {
    private:
	struct timeval start_tv;
	struct timeval stop_tv;
	struct rusage  start_usage;
	struct rusage  stop_usage;
    public:
	void Start() {
		gettimeofday(&start_tv, NULL);
		getrusage(RUSAGE_SELF, &start_usage);
	}
	void Stop() {
		getrusage(RUSAGE_SELF, &stop_usage);
		gettimeofday(&stop_tv, NULL);
	}
	void Print() {
		struct timeval real_time;
		struct timeval user_time, sys_time, cpu_time;
		
		real_time = timeval_sub(&stop_tv, &start_tv);
		user_time = timeval_sub(&stop_usage.ru_utime, &start_usage.ru_utime);
		sys_time = timeval_sub(&stop_usage.ru_stime, &start_usage.ru_stime);
		cpu_time = timeval_add(&user_time, &sys_time);
		
		cout << "real: " << real_time;
		cout << "    user+sys: " << user_time << "+" << sys_time << "=" << cpu_time;
		cout << endl;
	}
};


/*
 * 
 * Test 1
 * 
 */
void test1(const char *label, QpLockBase *lock, int iter)
{
	TimeMeasure tme;
	int i;
	
	cout << "test 1 " << label << " ";
	cout.flush();
	tme.Start();
	for (i = 0; i < iter; i++) {
		lock->Lock();
		lock->Unlock();
	}
	tme.Stop();
	tme.Print();
}

void test1_rw_read(const char *label, QpRWLockBase *lock, int iter)
{
	TimeMeasure tme;
	int i;
	
	cout << "test 1 " << label << " ";
	cout.flush();
	tme.Start();
	for (i = 0; i < iter; i++) {
		lock->ReadLock();
		lock->Unlock();
	}
	tme.Stop();
	tme.Print();
}

void test1_rw_write(const char *label, QpRWLockBase *lock, int iter)
{
	TimeMeasure tme;
	int i;
	
	cout << "test 1 " << label << " ";
	cout.flush();
	tme.Start();
	for (i = 0; i < iter; i++) {
		lock->WriteLock();
		lock->Unlock();
	}
	tme.Stop();
	tme.Print();
}

void test1_pthread_mutex(const char *label, int iter)
{
	TimeMeasure tme;
	int i;
	pthread_mutex_t m;
	
	pthread_mutex_init(&m, NULL);
	cout << "test 1 " << label << " ";
	cout.flush();
	tme.Start();
	for (i = 0; i < iter; i++) {
		if (pthread_mutex_lock(&m) != 0)
			exit(1);
		if (pthread_mutex_unlock(&m) != 0)
			exit(1);
	}
	tme.Stop();
	tme.Print();
}

void test1_tas_mutex(const char *label, int iter)
{
	TimeMeasure tme;
	int i;
	int lock = 0;
	
	cout << "test 1 " << label << " ";
	cout.flush();
	tme.Start();
	for (i = 0; i < iter; i++) {
		while (qp_arch_testandset(&lock))
			;
		qp_arch_release(&lock);
	}
	tme.Stop();
	tme.Print();
}

/*
 * 
 * Test 2
 * 
 */
class Test2Thread: public QpThread {
    private:
	QpLockBase *t_lock;
	int 	   t_iter;
    public:
	Test2Thread(QpLockBase *lock, int iter): t_lock(lock), t_iter(iter) {}
	virtual ~Test2Thread() {Join();}
	virtual void Main() {
		for (int i = 0; i < t_iter; i++) {
			QpSLock lock(t_lock);
			/* critical section with some code - few instructions 
			   maybe will be optimized (removed) by compiler */
			int j = t_iter;
			j++;
		}
	}
};

void test2(const char *label, QpLockBase *lock, int iter, int thr)
{
	TimeMeasure tme;
	vector<Test2Thread *> threads(thr);
	int i;
	
	cout << "test 2 " << label << " ";
	cout.flush();
	tme.Start();
	for (i = 0; i < thr; i++)
		threads[i] = new Test2Thread(lock, iter);
	for (i = 0; i < thr; i++)
		threads[i]->Start();
	for (i = 0; i < thr; i++)
		delete threads[i];
	tme.Stop();
	tme.Print();
}


/*
 * 
 * Test 3
 * 
 */
class Test3Thread: public QpThread {
    private:
	QpLockBase *t_lock;
	QpCond     *t_cond;
	int 	   t_iter;
	int	   t_id;
	int	   t_maxid;
	int	   *t_token;
	int	   t_trylock;
    public:
	Test3Thread(const char *name, QpLockBase *lock, QpCond *cond,
		    int iter, int id, int maxid, int *token, int trylock = 0):
		QpThread(NULL, QP_PRIO_DFL, 0, name),
		t_lock(lock), t_cond(cond), t_iter(iter), t_id(id), 
		t_maxid(maxid), t_token(token), t_trylock(trylock) {}
	virtual ~Test3Thread() {Join();}
	
	virtual void Main() {
		int i, lock_held;
		
		for (i = 0; i < t_iter; i++) {
			lock_held = 0;
			
			if (t_trylock) {
				for (int j = 0; j < 1; j++) {
					if ((lock_held = t_lock->TryLock())) {
						break;
					}
				}
			}
			if (!lock_held) {
				t_lock->Lock();
			}
			while (*t_token != t_id) {
				t_cond->Wait();
			}
			*t_token = (*t_token + 1) % t_maxid;
			cout.flush();
			t_cond->Broadcast();
			t_lock->Unlock();
		}
	}
};

void test3(const char *label, QpLockBase *lock, int iter, int thr, int trylock = 0)
{
	TimeMeasure tme;
	vector<Test3Thread *> threads(thr);
	QpCond *cond;
	int i;
	int token;
	char name[2];
	
	cout << "test 3 " << label << " ";
	cout.flush();
	
	token = 0;
	cond = new QpCond(lock);
	name[0] = '0';
	name[1] = 0;
	tme.Start();
	for (i = 0; i < thr; i++) {
		threads[i] = new Test3Thread(name, lock, cond, iter, i, thr, 
					     &token, trylock);
		name[0]++;
	}
	for (i = 0; i < thr; i++)
		threads[i]->Start();
	for (i = 0; i < thr; i++)
		delete threads[i];
	tme.Stop();
	tme.Print();
	free(cond);
}

int main(int argc, char *argv[])
{
	QpInit qp_init;
	int c;
	int iter = 10000;
	int thr = 8;
	int try_lock_test = 0;
	
	QpSpinLock	spin_lock;
	QpMutex		mutex;
	QpMutexEx	mutex_ex;
	QpSem		sem;
	QpAsyncSafeSem	async_sem;
	QpRWLock	rw_lock;
	QpRWLockEx	rw_lock_ex;
	
	while ((c = getopt(argc, argv, "i:t:y")) != EOF) {
		switch (c) {
		    case 'i':
			iter = atoi(optarg);
			break;
		    case 't':
			thr = atoi(optarg);
			break;
		    case 'y':
			try_lock_test = 1;
			break;
		    default:
			printf("usage: %s [-i iter] [-t threads]\n", argv[0]);
			exit(1);
		}
	}

	test1_pthread_mutex("pthread_mutex  ", iter);
	test1_tas_mutex("tas mutex      ", iter);
	test1("QpSpinLock     ", &spin_lock, iter);
	test1("QpMutex        ", &mutex, iter);
	test1("QpMutexEx      ", &mutex_ex, iter);
	test1("QpSem          ", &sem, iter);
	test1("QpAsyncSafeSem ", &async_sem, iter);
	test1_rw_read("QpRWLock Read  ", &rw_lock, iter);
	test1_rw_write("QpRWLock Write ", &rw_lock, iter);
	test1_rw_read("QpRWLockEx Read", &rw_lock_ex, iter);
	test1_rw_write("QpRWLockEx Write", &rw_lock_ex, iter);
	cout << endl;
	
	test2("QpSpinLock     ", &spin_lock, iter, thr);
	test2("QpMutex        ", &mutex, iter, thr);
	test2("QpMutexEx      ", &mutex_ex, iter, thr);
	test2("QpSem          ", &sem, iter, thr);
	test2("QpAsyncSafeSem ", &async_sem, iter, thr);

	cout << endl << "possibly lower the number of iterations (-i 1000)" << endl;

	test3("QpSpinLock     ", &spin_lock, iter, thr);
	test3("QpMutex        ", &mutex, iter, thr);
	test3("QpMutexEx      ", &mutex_ex, iter, thr);
	test3("QpSem          ", &sem, iter, thr);
	test3("QpAsyncSafeSem ", &async_sem, iter, thr);

	if (try_lock_test) {
		cout << endl << "next test can be very slow (-i 100 recommended)" << endl;
	
		test3("QpSpinLock     ", &spin_lock, iter, thr, 1);
		test3("QpMutex        ", &mutex, iter, thr, 1);
		test3("QpMutexEx      ", &mutex_ex, iter, thr, 1);
		test3("QpSem          ", &sem, iter, thr, 1);
		test3("QpAsyncSafeSem ", &async_sem, iter, thr, 1);
	}

	cout << endl << "done" << endl;
	return 0;
}

