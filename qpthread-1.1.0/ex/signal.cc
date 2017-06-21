#include "ex.h"
#include <signal.h>
#include <math.h>
#include <setjmp.h>
#include <qpthr/qp.h>
#include <qpthr/qp_work.h>

// XJB - signals are not supported on Windows
#ifdef WIN32
#error "Signals are not supported on Windows!"
#endif // WIN32

int sigtest_run = 1;

void sig_term_func(int sig, void *arg)
{
	cout << "signal " << sig << " deliverd to " << QpThread::CurrentSeq() << " pid " << getpid() << endl;
	sigtest_run = 0;
}

void sig_fpe_func()
{
	cout << "signal FPE deliverd to " << QpThread::CurrentSeq() << " pid " << getpid() << endl;
}

void sig_segv_handler(int sig, void *arg)
{
	printf("segm. fault (*dp = 1)\n");
	sigjmp_buf *buf = (sigjmp_buf *) arg;
	siglongjmp(*buf, 1);
}

class SigTest: public QpThread {
    public:
	~SigTest() {Join();}
	virtual void Main() {
		double d = 0, dd = 5, *dp = NULL;
		
		QpWork_p0 *r = new QpWork_p0(sig_fpe_func);
		
		cout << "Thread " << ThreadSeq() << " running " << getpid() << endl;
		QpSignal::RegisterSignalFunc(SIGTERM, sig_term_func);
		QpSignal::RegisterSignalFunc(SIGFPE, r);
		
		cout << "/0" << endl;
		d = dd / sin(0);

		
		sigjmp_buf jumpbuffer;
		QpRegItem ff = QpSignal::RegisterSignalFunc(SIGSEGV, sig_segv_handler, &jumpbuffer);
#if 1
		if (sigsetjmp(jumpbuffer, 1) == 0) {
			*dp = 1;
		}
#endif
		QpSignal::UnregisterSignalFunc(ff);
		
		cout << d << endl;
		cout << "going to sleep" << endl;
#if 1
		while (sigtest_run) {
			cout << "sleep" << endl;
			Sleep(1000);
		}
#endif
		cout << "Thread " << ThreadSeq() << " main done" << endl;
	}
};


int main()
{
	cout << "init" << endl;
	QpInit qp_init(QpInit::QP_SIGNAL);
	cout << "init done" << endl;
	cout << "Main " << QpThread::CurrentSeq() << " pid " << getpid() << endl;
	QpSignal::InstallHandler(SIGTERM, QpSignal::HANDLER);
 	QpSignal::InstallHandler(SIGFPE,  QpSignal::HANDLER);
 	QpSignal::InstallHandler(SIGSEGV, QpSignal::HANDLER);
	QpSignal::InstallHandler(SIGQUIT, QpSignal::EXIT);
	QpSignal::InstallHandler(SIGINT,  QpSignal::EXIT);
	SigTest t1;
	
	t1.Start();
	t1.Join();	
	cout << "main done" << endl;
	return 0;
}
