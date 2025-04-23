#ifndef UASSTATEMACHINE_HPP
#define UASSTATEMACHINE_HPP

#include <QObject>
#include <QDebug>

/**
 * @class UASState
 * @brief Defines the possible states for the Unmanned Aerial System (UAS)
 * 
 * This class provides an enumeration of the possible flight states
 * that the UAS can be in. The State enum is registered with the Qt
 * meta-object system to make it available to QML.
 */
class UASState : public QObject
{
    Q_OBJECT
    
public:
    /**
     * @enum State
     * @brief The various states a UAS can be in
     * 
     * @value Landed The UAS is on the ground
     * @value TakingOff The UAS is in the process of taking off
     * @value Flying The UAS is flying normally
     * @value FlyingToWaypoint The UAS is navigating to a specific waypoint
     * @value Loitering The UAS is circling around a point
     * @value Landing The UAS is in the process of landing
     */
    enum State {
        Landed,
        TakingOff,
        Flying,
        FlyingToWaypoint,
        Loitering,
        Landing
    };
    Q_ENUM(State)
};

/**
 * @class UASStateMachine
 * @brief Manages the state transitions for the Unmanned Aerial System
 * 
 * This class implements a state machine that controls the valid transitions
 * between different flight states of the UAS. It ensures that state transitions
 * follow a logical sequence (e.g., the UAS can only take off if it's landed).
 */
class UASStateMachine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(UASState::State currentState READ currentState NOTIFY currentStateChanged)
    
public:
    /**
     * @brief Constructs a UASStateMachine
     * @param parent The parent QObject
     */
    explicit UASStateMachine(QObject* parent = nullptr);
    
    /**
     * @brief Destructor
     */
    virtual ~UASStateMachine();
    
    /**
     * @brief Gets the current state of the UAS
     * @return The current state
     */
    UASState::State currentState() const;

    /**
     * @brief Directly sets the current state
     * @param state The new state to set
     * @return True if success, false otherwise
     * 
     * This method is primarily used by the TelemetryDataSimulator to
     * update the state machine based on simulation events.
     */
    Q_INVOKABLE bool setCurrentState(UASState::State state);
    
signals:
    /**
     * @brief Emitted when the UAS state changes
     * @param state The new state
     */
    void currentStateChanged(UASState::State state);
    
private:
    /** @brief The current state of the UAS */
    UASState::State m_currentState;
};

#endif // UASSTATEMACHINE_HPP
