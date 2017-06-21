#include "ex.h"
#include <qpthr/qp.h>

QpTSD tsd;


class TsdThread: public QpThread {
    public:
	virtual ~TsdThread() {Join();}
	virtual void Main() {
		cout << CurrentSeq() << " Set " << CurrentSeq() << endl;
		tsd.Set((void *) CurrentSeq());
		Sleep(1000);
		cout << CurrentSeq() << " Get " << (unsigned long) tsd.Get() << endl;
	}
};


int main()
{
	QpInit qp_init;
	TsdThread t1, t2, t3, t4;
	
	t1.Start();
	t2.Start();
	t3.Start();
	t4.Start();
	
	return 0;
}

