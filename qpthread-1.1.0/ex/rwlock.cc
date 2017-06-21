#include "ex.h"
#include <qpthr/qp.h>


QpRWLock	rw_lock;
QpRWLockEx	rw_lock_ex;
int 		nr1 = 0;
int		nr2 = 0;

int	iter = 100;
bool	test_run = true;

int	test_nr = 1;


class WriterT1: public QpThread {
    private:
	int w_id;
    public:
	WriterT1(int id): w_id(id) {}
	virtual ~WriterT1()	{ Join(); }
	
	virtual void Main() {
		for (int i = 0; i < iter; i++) {
			QpWriteSLock lock(rw_lock);
			
			Sleep(100);
			if (i % 10 == 0) {
				nr1++;
				Sleep(100);
				nr2++;
				cout << " W" << w_id << "/" << nr1;
				cout.flush();
			} else {
				cout << " w" << w_id << "/" << nr1;
				cout.flush();
			}
		}
		test_run = false;
	}
};

class WriterT2: public QpThread {
    private:
	int	w_id;
    public:
	WriterT2(int id): w_id(id) {}
	virtual ~WriterT2()	{ Join(); }
	
	virtual void Main() {
		for (int i = 0; i < iter; i++) {
			QpReadForWriteSLock lock(rw_lock_ex);
			Sleep(100);
			if (i % 10 == 0) {
				rw_lock_ex.PromoteWriteLock();
				nr1++;
				Sleep(100);
				nr2++;
				cout << " W" << w_id << "/" << nr1;
				cout.flush();
			} else {
				cout << " w" << w_id << "/" << nr1;
				cout.flush();
			}
		}
		test_run = false;
	}
};

class Reader: public QpThread {
    private:
	int 		r_id;
	QpRWLockBase	*rw_lock;
    public:
	Reader(int id, QpRWLockBase *lock): r_id(id), rw_lock(lock) {}
	virtual ~Reader()	{ Join(); }
	
	virtual void Main() {
		int r1, r2;
		
		while(test_run) {
			QpReadSLock lock(rw_lock);
			r1 = nr1;
			Sleep(100);
			r2 = nr2;
			
			if (r1 != r2) {
				cerr << "ERROR: r1 != r2" << endl;
				exit(1);
			}
			cout << " " << r_id;
			cout.flush();
		}
	}
};


int main(int argc, char *argv[])
{
	bool second_writer = false;
	int c;
	QpInit qp_init;
	
	
	while ((c = getopt(argc, argv, "i:w12")) != EOF) {
		switch (c) {
		    case 'i':
			iter = atoi(optarg);
			break;
		    case '1':
			test_nr = 1;
			break;
		    case '2':
			test_nr = 2;
			break;
		    case 'w':
			second_writer = true;
			break;
		}
	}

	
	if (test_nr == 1) {
		WriterT1 *w1, *w2;
		Reader *r1, *r2, *r3, *r4;
		
		w1 = w2 = NULL;
		r1 = r2 = r3 = r4 = NULL;
		
		w1 = new WriterT1(1);
		if (second_writer)
			w2 = new WriterT1(2);
		r1 = new Reader(1, &rw_lock);
		r2 = new Reader(2, &rw_lock);
		r3 = new Reader(3, &rw_lock);
		r4 = new Reader(4, &rw_lock);

		w1->Start();
		if (w2)
			w2->Start();
		r1->Start();
		r2->Start();
		r3->Start();
		r4->Start();
		w1->Join();
		if (w2)
			w2->Join();
		
		delete r1;
		delete r2;
		delete r3;
		delete r4;
		if (w2)
			delete w2;
		delete w1;
		
	} else if (test_nr == 2) {
		WriterT2 *w1, *w2;
		Reader *r1, *r2, *r3, *r4;
		
		w1 = w2 = NULL;
		r1 = r2 = r3 = r4 = NULL;
		
		w1 = new WriterT2(1);
		if (second_writer)
			w2 = new WriterT2(2);
		r1 = new Reader(1, &rw_lock_ex);
		r2 = new Reader(2, &rw_lock_ex);
		r3 = new Reader(3, &rw_lock_ex);
		r4 = new Reader(4, &rw_lock_ex);

		w1->Start();
		if (w2)
			w2->Start();
		r1->Start();
		r2->Start();
		r3->Start();
		r4->Start();
		w1->Join();
		if (w2)
			w2->Join();
		
		delete r1;
		delete r2;
		delete r3;
		delete r4;
		if (w2)
			delete w2;
		delete w1;
		
	}
	cout << endl;
	return 0;
}

