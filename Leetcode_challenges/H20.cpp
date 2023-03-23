class H2O {
private:
    //limiters for atoms inside molecule
    const int h2 = 2;
    const int o1 = 1;
    //mutex and variables with limited access by it
    mutex mtx;
    int h = 0;
    int o = 0;
    condition_variable cv;
    //check if the molecule is complete. Reset for another one.
    void check_set() {
        if (h == h2 && o == o1){
            h = 0;
            o = 0;
        }
    }
public:
    H2O() {
    }

    void hydrogen(function<void()> releaseHydrogen) {
        unique_lock<mutex> ulck(mtx);
        //barrier
        cv.wait(ulck, [=] {return h < h2;} );
        //critical section
        // releaseHydrogen() outputs "H". Do not change or remove this line.
        releaseHydrogen();
        h++;
        check_set();
        //make resource available to other threads
        cv.notify_all();
    }

    void oxygen(function<void()> releaseOxygen) {
        unique_lock<mutex> ulck(mtx);
        //barrier
        cv.wait(ulck, [=] {return o < o1;} );
        //critical section
        // releaseOxygen() outputs "O". Do not change or remove this line.
        releaseOxygen();
        o++;
        check_set();
        //make resource available to other threads
        cv.notify_all();
    }
};