#include "ex.h"
#include <stdlib.h>
#include <qpthr/qp.h>
#include <qpthr/qp_work.h>


void f()
{
	QpThread::Sleep(100);  /* 0.1s */
	cout << QpThread::CurrentSeq() << " ";
	cout.flush();
}

class LongJob: public QpRunnable {
    public:
	virtual void Main() {
		cout << "LongJob running" << endl;
		QpThread::Sleep(1000);
		cout << "LongJob end" << endl;
	}
};

int main(int argc, char *argv[])
{
	QpInit qp_init;
	int c, i;
	bool on_demand = false;
	int iter = 100;
	int threads = 5;
	int life_time = 5;
	
	while ((c = getopt(argc, argv, "di:t:l:")) != EOF) {
		switch (c) {
		    case 'd':
			on_demand = true;
			break;
		    case 'i':
			iter = atoi(optarg);
			break;
		    case 't':
			threads = atoi(optarg);
			break;
		    case 'l':
			life_time = atoi(optarg);
			break;
		}
	};
	cout << on_demand << endl;
	QpThreadPool tpool(QP_PRIO_DFL, 0, threads, on_demand, life_time * 1000);
	cout << endl << "threads = " << tpool.CurrentThreads() << endl;

	cout << "test 1" << endl;
	for (i = 0; i < iter; i++) {
		QpWork_p0 work(&f);
		tpool.Do(&work);
		tpool.Wait();
	}
	
	cout << endl << "test 2" << endl;
	list<QpWork_p0 *> work_list;
	for (i = 0; i < iter; i++) {
		QpWork_p0 *work = new QpWork_p0(&f);
		work_list.push_back(work);
		tpool.Do(work);
	}
	while (work_list.size()) {
		tpool.Wait(work_list.back());
		work_list.pop_back();
	}
	cout << endl << "threads = " << tpool.CurrentThreads() << endl;
	QpThread::Sleep(5500);
	cout << endl << "threads = " << tpool.CurrentThreads() << endl;
	
	
	cout << endl << "test 3" << endl;
	LongJob long_job;
	tpool.Do(&long_job);
	try {
		tpool.Wait(&long_job, 500);
	}
	catch (QpTimeoutException &e) {
		cout << "timeout" << endl;
	}
	tpool.Wait(&long_job);
	
	cout << endl << "done" << endl;
	
	return 0;
}

