#ifndef STATE_H
#define STATE_H

enum State
{
    PRELAUNCH,
    LAUNCH_READY,
    POWERED_FLIGHT,
    UNPOWERED_FLIGHT,
    BALLISTIC_DESCENT,
    POST_DROGUE,
    POST_MAIN,
    TEST_READY,
    END
};

extern enum State current_state;

#endif