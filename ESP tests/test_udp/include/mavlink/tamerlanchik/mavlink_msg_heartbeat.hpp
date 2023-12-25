// MESSAGE HEARTBEAT support class

#pragma once

namespace mavlink {
namespace tamerlanchik {
namespace msg {

/**
 * @brief HEARTBEAT message
 *
 * The heartbeat message shows that a system or component is present and responding. The type and autopilot fields (along with the message component id), allow the receiving system to treat further messages from this system appropriately (e.g. by laying out the user interface based on the autopilot). This microservice is documented at https://mavlink.io/en/services/heartbeat.html
 */
struct HEARTBEAT : mavlink::Message {
    static constexpr msgid_t MSG_ID = 0;
    static constexpr size_t LENGTH = 2;
    static constexpr size_t MIN_LENGTH = 2;
    static constexpr uint8_t CRC_EXTRA = 73;
    static constexpr auto NAME = "HEARTBEAT";


    uint8_t type; /*<  Vehicle or component type. For a flight controller component the vehicle type (quadrotor, helicopter, etc.). For other components the component type (e.g. camera, gimbal, etc.). This should be used in preference to component id for identifying the component type. */
    uint8_t check_value; /*<  Integer value which vehicle must be return modified in a known method. */


    inline std::string get_name(void) const override
    {
            return NAME;
    }

    inline Info get_message_info(void) const override
    {
            return { MSG_ID, LENGTH, MIN_LENGTH, CRC_EXTRA };
    }

    inline std::string to_yaml(void) const override
    {
        std::stringstream ss;

        ss << NAME << ":" << std::endl;
        ss << "  type: " << +type << std::endl;
        ss << "  check_value: " << +check_value << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << type;                          // offset: 0
        map << check_value;                   // offset: 1
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> type;                          // offset: 0
        map >> check_value;                   // offset: 1
    }
};

} // namespace msg
} // namespace tamerlanchik
} // namespace mavlink
