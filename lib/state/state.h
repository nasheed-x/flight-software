#ifndef STATE_H
#define STATE_H

enum State
{
    PRELAUNCH,
    LAUNCH_READY,
    MOTOR_BURN,
    BURNOUT,
    APOGEE,
    POST_DROGUE,
    POST_MAIN,
    END
};

#endif