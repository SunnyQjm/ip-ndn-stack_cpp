#ifndef NFD_DAEMON_FACE_PCAP_HELPER_HPP
#define NFD_DAEMON_FACE_PCAP_HELPER_HPP

#include <iostream>
#include <pcap/pcap.h>
#include <unistd.h>
#include <tuple>
#include "packet.h"
#include <chrono>
#include "JSONCPPHelper.h"
#include <boost/throw_exception.hpp>

using namespace std;

namespace IP_NDN_STACK {
    namespace pcap {
/**
 * @brief Helper class for dealing with libpcap handles.
 */
        class PcapHelper {
        public:
            class Error : public std::runtime_error {
            public:
                explicit
                Error(const std::string &what)
                        : std::runtime_error(what) {
                }
            };

            /**
             * @brief Create a libpcap context for live packet capture on a network interface.
             * @throw Error on any error
             * @sa pcap_create(3pcap)
             */
            explicit
            PcapHelper(const std::string &configFilePath);

            ~PcapHelper();

            /**
             * @brief Start capturing packets.
             * @param dlt The link-layer header type to be used.
             * @throw Error on any error
             * @sa pcap_activate(3pcap), pcap_set_datalink(3pcap)
             */
            void
            activate(int dlt = -1);

            /**
             * @brief Stop capturing and close the handle.
             * @sa pcap_close(3pcap)
             */
            void
            close();

            /**
             * @brief Obtain a file descriptor that can be used in calls such as select(2) and poll(2).
             * @pre activate() has been called.
             * @return A selectable file descriptor. It is the caller's responsibility to close the fd.
             * @throw Error on any error
             * @sa pcap_get_selectable_fd(3pcap)
             */
            int
            getFd() const;

            /**
             * @brief Get last error message.
             * @return Human-readable explanation of the last libpcap error.
             * @warning The behavior is undefined if no error occurred.
             * @sa pcap_geterr(3pcap)
             */
            std::string
            getLastError() const;

            /**
             * @brief Get the number of packets dropped by the kernel, as reported by libpcap.
             * @throw Error on any error
             * @sa pcap_stats(3pcap)
             */
            size_t
            getNDropped() const;

            /**
             * @brief Install a BPF filter on the receiving socket.
             * @param filter Null-terminated string containing the BPF program source.
             * @pre activate() has been called.
             * @throw Error on any error
             * @sa pcap_setfilter(3pcap), pcap-filter(7)
             */
            void
            setPacketFilter(const string &filter) const;

            std::tuple<const tuple_p, size_t, std::string>
            readNextPacketAfterDecode();

            /**
           * @brief Read the next packet captured on the interface.
           * @return If successful, returns a tuple containing a pointer to the received packet
           *         (including the link-layer header) and the size of the packet; the third
           *         element must be ignored. On failure, returns a tuple containing nullptr,
           *         0, and the reason for the failure.
           * @warning The returned pointer must not be freed by the caller, and is valid only
           *          until the next call to this function.
           * @sa pcap_next_ex(3pcap)
           */
            std::tuple<const uint8_t *, size_t, std::string>
            readNextPacket() const;

            /**
             * 获取当前时间戳
             */
            long getCurTime();

            pcap_t * getPcap() {
                return m_pcap;
            }

            explicit
            operator pcap_t *() const {
                return m_pcap;
            }

        private:
            pcap_t *m_pcap;
            bpf_u_int32 net;
        };
    }
}
#endif // NFD_DAEMON_FACE_PCAP_HELPER_HPP