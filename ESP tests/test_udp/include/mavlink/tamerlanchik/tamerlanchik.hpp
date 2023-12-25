/** @file
 *	@brief MAVLink comm protocol generated from tamerlanchik.xml
 *	@see http://mavlink.org
 */

#pragma once

#include <array>
#include <cstdint>
#include <sstream>

#ifndef MAVLINK_STX
#define MAVLINK_STX 253
#endif

#include "../message.hpp"

namespace mavlink {
namespace tamerlanchik {

/**
 * Array of msg_entry needed for @p mavlink_parse_char() (through @p mavlink_get_msg_entry())
 */
constexpr std::array<mavlink_msg_entry_t, 1> MESSAGE_ENTRIES {{ {0, 73, 2, 2, 0, 0, 0} }};

//! MAVLINK VERSION
constexpr auto MAVLINK_VERSION = 3;


// ENUM DEFINITIONS


/** @brief MAVLINK component type reported in HEARTBEAT message. Flight controllers must report the type of the vehicle on which they are mounted (e.g. MAV_TYPE_OCTOROTOR). All other components must report a value appropriate for their type (e.g. a camera must use MAV_TYPE_CAMERA). */
enum class MAV_TYPE : uint8_t
{
    GENERIC=0, /* Generic micro air vehicle | */
    FIXED_WING=1, /* Fixed wing aircraft. | */
    QUADROTOR=2, /* Quadrotor | */
};

//! MAV_TYPE ENUM_END
constexpr auto MAV_TYPE_ENUM_END = 3;


} // namespace tamerlanchik
} // namespace mavlink

// MESSAGE DEFINITIONS
#include "./mavlink_msg_heartbeat.hpp"

// base include

