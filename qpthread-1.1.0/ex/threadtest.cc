#include "ex.h"
#include <qpthr/qp.h>

class CreateTest: public QpThread {
    public:
	virtual ~CreateTest() {Join();}
	virtual void Main() {
		cout << "x ";
		cout.flush();
		Yield();
		cout << "y ";
		cout.flush();
	}
};


int main(int argc, char *argv[])
{
	QpInit qp_init;
	int c, i, j;
	int iter = 1000;
	int concurency = 100;
	
	while ((c = getopt(argc, argv, "i:c:")) != EOF) {
		switch (c) {
		    case 'i':
			iter = atoi(optarg);
			break;
		    case 'c':
			concurency = atoi(optarg);
			break;
		}
	}
	typedef CreateTest *CreateTest_t;
	CreateTest_t *ct = new CreateTest_t[concurency];
	
	for (i = 0; i < iter; i++) {
		cout << endl << i << " --------" << endl;
		for (j = 0; j < concurency; j++) {
			ct[j] = new CreateTest;
			if (!ct[j]->IsAlive()) {
				ct[j]->Start();
			}
		}
		for (j = 0; j < concurency; j++) {
			delete ct[j];
		}
	}
	cout << endl;
	return 0;
}

