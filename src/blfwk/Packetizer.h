/*
 * Copyright (c) 2013-14, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _Packetizer_h_
#define _Packetizer_h_

#include "bootloader_common.h"
#include "bootloader/bl_peripheral.h"

#include <time.h>

//! @addtogroup host_packetizers
//! @{

namespace blfwk
{
// Forward declarations.
class Peripheral;

//! @brief Packetizer status codes.
enum _packetizer_status
{
    kStatus_NoPingResponse = MAKE_STATUS(kStatusGroup_Packetizer, 0),
    kStatus_InvalidPacketType = MAKE_STATUS(kStatusGroup_Packetizer, 1),
    kStatus_InvalidCRC = MAKE_STATUS(kStatusGroup_Packetizer, 2),
    kStatus_NoCommandResponse = MAKE_STATUS(kStatusGroup_Packetizer, 3)
};

/*!
 * @brief Interface class for packetization of commands and data.
 */
class Packetizer
{
public:
    Packetizer(Peripheral *peripheral, uint32_t packetTimeoutMs)
        : m_peripheral(peripheral)
        , m_packetTimeoutMs(packetTimeoutMs)
        , m_version()
        , m_options(0)
        , m_isAbortEnabled(false)
    {
    }

    virtual ~Packetizer(){};

    //! @brief Read a packet.
    virtual status_t readPacket(uint8_t **packet, uint32_t *packetLength, packet_type_t packetType) = 0;

    //! @brief Write a packet.
    virtual status_t writePacket(const uint8_t *packet, uint32_t byteCount, packet_type_t packetType) = 0;

    //! @brief Abort data phase.
    virtual void abortPacket() = 0;

    //! @brief Send framing packet ack.
    virtual void sync() = 0;

    //! @brief Finalize.
    virtual void finalize() = 0;

    //! @brief Enable simulator command processor pump.
    virtual void enableSimulatorPump() = 0;

    //! @brief Pump simulator command processor.
    virtual void pumpSimulator() = 0;

    //! @brief Set aborted flag.
    //!
    //! Used for out-of-band flow control for simulator.
    virtual void setAborted(bool aborted) = 0;

    //! @brief Return the max packet size.
    virtual uint32_t getMaxPacketSize() = 0;

    //! @brief Peripheral accessor.
    virtual Peripheral *getPeripheral() { return m_peripheral; }
    //! @brief Get Framing Protocol Version
    standard_version_t getVersion() { return m_version; }
    //! @brief Get Framing Protocol Options
    uint16_t getOptions() { return m_options; }
    //! @brief Set abort packet check enable.
    void setAbortEnabled(bool isEnabled) { m_isAbortEnabled = isEnabled; }
    //! @biref Check if abort data phase is enabled.
    bool isAbortEnabled() { return m_isAbortEnabled; }
protected:
    Peripheral *m_peripheral;     //!< Peripheral to send/receive bytes on.
    standard_version_t m_version; //!< Framing protocol version.
    uint16_t m_options;           //!< Framing protocol options bitfield.
    clock_t m_startTime;          //!< Beginning time of packet transaction.
    uint32_t m_packetTimeoutMs;
    bool m_isAbortEnabled; //!< True if allowing abort packet. Not used by all packetizers.
};

} // namespace blfwk

//! @}

#endif // _Packetizer_h_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
