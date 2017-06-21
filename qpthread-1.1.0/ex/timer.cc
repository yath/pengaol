#include "ex.h"
#include <qpthr/qp.h>


class TimerTest: public QpRunnable {
	QpMonitor tt_monitor;
	int 	  tt_count;
    public:
	TimerTest(): tt_count(0) {}
	virtual void Main() {
		cout << "TimerTest" << endl;
		QpSLock lock(&tt_monitor);
		tt_count++;
		tt_monitor.Broadcast();
	}
	void WaitForCount(int c) {
		QpSLock lock(&tt_monitor);
		while (tt_count < c)
			tt_monitor.Wait();
	}
};

int main(int argc, char *argv[])
{
	QpInit qp_init(QpInit::QP_TIMER);
	TimerTest tt;
	QpRegItem ri;
	int c;
	int infinite = 0;
	
	while ((c = getopt(argc, argv, "i")) != EOF) {
		switch (c) {
		    case 'i':
			infinite = 1;
			break;
		}
	}
	ri = QpTimer::Register(&tt, 500, 10);
	tt.WaitForCount(5);
	QpTimer::Unregister(ri);
	QpThread::Sleep(1000);
	cout << endl;
	ri = QpTimer::Register(&tt, 500, 1);
	QpThread::Sleep(1000);
//	QpTimer::Unregister(ri);	/* we can skip this */
	
	ri = QpTimer::Register(&tt, 100, 0);
	QpThread::Sleep(300);
	QpTimer::Unregister(ri);
	
	if (infinite) {
		cout << "press ctrl-c to end" << endl;
		ri = QpTimer::Register(&tt, 500, QpTimer::INFINITE_SHOTS);
		QpThread::Sleep();
	}

	// XJB added to omit warning "function should return a value"
	return 0;
}
