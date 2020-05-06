#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <QObject>
#include <pprzlink/IvyLink.h>
#include <pprzlink/Message.h>
#include <memory>

class PprzDispatcher : public QObject
{
    Q_OBJECT

    static PprzDispatcher* singleton;
    explicit PprzDispatcher(QObject *parent = nullptr);

public:
    static PprzDispatcher* get(QObject *parent = nullptr) {
        if(!singleton) {
            singleton = new PprzDispatcher(parent);
        }
        return singleton;
    }

    void start();

    void sendMessage(pprzlink::Message);
    std::shared_ptr<pprzlink::MessageDictionary> getDict() {return dict;}

signals:
    void flight_param(pprzlink::Message);
    void ap_status(pprzlink::Message);
    void nav_status(pprzlink::Message);
    void circle_status(pprzlink::Message);
    void segment_status(pprzlink::Message);
    void engine_status(pprzlink::Message);
    void waypoint_moved(pprzlink::Message);
    void dl_values(pprzlink::Message);
    void telemetry_status(pprzlink::Message);


public slots:

private:

    using sig_ptr_t = decltype(&PprzDispatcher::flight_param);

    void requestConfig(std::string ac_id);
    void bindDeftoSignal(std::string const &name, sig_ptr_t sig);

    std::shared_ptr<pprzlink::MessageDictionary> dict;
    std::unique_ptr<pprzlink::IvyLink> link;

    std::string pprzlink_id;

    bool first_msg;
    bool started;
};




#endif // DISPATCHER_H
