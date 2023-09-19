
class System{
    public:
        System(void);
        typedef enum States_e{
            DiodeSelecting = 0,
            CurrentCoarseSetting,
            CurrentFineSetting,
            TempCoarseSetting,
            TempFineSetting
        }State;
        void setState(State nextState);
        State getState(void);
    private:
        volatile State state;
};