#include "ex.h"
#include <qpthr/qp.h>

const int ITER = 2;
class SleepThr: public QpThread {
    public:
	virtual ~SleepThr() {Join();}
	virtual void Main() {
		for (int i = 0; i < ITER; i++) {
			cout << i << "    " << ThreadSeq() << " - Sleep ";
			cout.flush();
			QpThread::Sleep(1000);
			cout << ThreadSeq() << " OK"<< endl;
		}
	}
};


int main(int argc, char *argv[])
{
	QpInit qp_init;
	SleepThr ht;
	int i;
	
	ht.Start();
	ht.Join();

	ht.Start();
	for (i = 0; i < ITER; i++) {
		QpThread::Sleep(100);
		ht.WakeUp();
	}
	ht.Join();
#if 0
	SleepThr ht2;
	ht2.Start();
	for (i = 0; i < 10; i++) {
		QpThread::Sleep(100);
		ht2.WakeUp();
	}
	ht2.Join();
#endif
	return 0;
}

