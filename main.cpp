#include "visualcalculator.h"


class Singleton
{
public:

    static Singleton& instance() {

        static Singleton inst;
        return inst;
    }
    Singleton(const Singleton&) = delete;
    VisualCalculator& get() { return app_; }

private:

    Singleton() : app_() { }
    Singleton& operator= (const Singleton&) = delete;

    VisualCalculator app_;
};

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    Singleton& singleton = Singleton::instance();
    VisualCalculator& calculator = singleton.get();
    calculator.resize(350, 500);
    calculator.show();

    return a.exec();
    
}
