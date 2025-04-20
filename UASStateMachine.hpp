#ifndef UASSTATEMACHINE_HPP
#define UASSTATEMACHINE_HPP

#include <QObject>
#include <QDebug>

// Define UAS flight states as an enum
class UASState : public QObject
{
    Q_OBJECT
    
public:
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

class UASStateMachine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(UASState::State currentState READ currentState NOTIFY currentStateChanged)
    
public:
    explicit UASStateMachine(QObject* parent = nullptr);
    virtual ~UASStateMachine();
    
    UASState::State currentState() const;
    
    // State change commands
    Q_INVOKABLE void takeOff();
    Q_INVOKABLE void land();
    Q_INVOKABLE void loiter();
    Q_INVOKABLE void fly();
    
    // Make state setting available to TelemetryDataSimulator
    Q_INVOKABLE void setCurrentState(UASState::State state);
    
signals:
    void currentStateChanged(UASState::State state);
    
private:
    UASState::State m_currentState;
};

#endif // UASSTATEMACHINE_HPP
