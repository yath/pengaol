#include "ex.h"
#include <qpthr/qp.h>

class CancelTest: public QpThread {
    public:
	~CancelTest() { Join();}
	virtual void Main() {
		try {
			cout << "Cancel Test" << endl;
			while (1) {
				TestCancel();
				Sleep(100);
			}
		}
		catch (QpCancelException &e) {
			/* code when canceled */
			cout << "cancel end code - QpCancelException" << endl;
			throw QpCancelException(); /* rethrow */
		}
	}
};


int main()
{
	QpInit qp_init;
	CancelTest ct;
	
	ct.Start();
	QpThread::Sleep(300);
	ct.Cancel();
	ct.Join();

	try {
		ct.Raise();
	}
	catch (QpCancelException &e) {
		cout << e << " ok" << endl;
	}

	cout << "done" << endl;
	return 0;
}

