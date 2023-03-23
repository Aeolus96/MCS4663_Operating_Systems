class DiningPhilosophers {
    private:
    //mutex array for managing forks
    mutex mtx[5];
    
    //fixed fork positions for easy recall using philosopher id
    const int RightFork[5] = {4, 0, 1, 2, 3};
    const int LeftFork[5] = {0, 1, 2, 3, 4};
    
    
    public:
    DiningPhilosophers() {
        
    }

    void wantsToEat(int philosopher, function<void()> pickLeftFork, function<void()> pickRightFork, function<void()> eat, function<void()> putLeftFork, function<void()> putRightFork) {
		//Grab both forks at the same time but only if both are available
        //Allows only two philosophers sitting opposite to each other to pick up forks and eat at the same time
        //Scoped lock prevents deadlocks by design and checks for 
        //both locks' availability but does not acquire them unless both are available
        scoped_lock ForkLock(mtx[LeftFork[philosopher]], mtx[RightFork[philosopher]]);
        //Eating ritual...
        pickLeftFork(); 
        pickRightFork(); 
        eat(); 
        putLeftFork(); 
        putRightFork();
        //End of scope automatically releases the fork locks
        //because scope_lock uses RAII-style mechanism
    }
};