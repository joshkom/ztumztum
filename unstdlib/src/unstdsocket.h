#ifndef UNSTDLIB_UNSTDSOCKET_H
#define UNSTDLIB_UNSTDSOCKET_H

#include "unstdinttypes.h"
#include "unstdbool.h"
#include "unstdstring.h"
#include "_unstdcommon.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>


/**
 * @brief Validates if the given IP address string is a valid IPv4 or IPv6 address.
 * @param ip_address_arg Should be a pointer to a valid, null-terminated heap-allocated / c-array buffer
 *                       containing the IP address string to validate.
 * @returns A number (u8t) indicating the result of the validation.
 * @retval [0] Failure. The given IP address is not valid.
 * @retval [1] Success. The given IP address is valid.
 * @retval [2] Insufficient parameter. `ip_address_arg` is NULL. See `ip_address_arg`.
 * @retval [3] Insufficient parameter. `ip_address_arg` is an empty string. See `ip_address_arg`.
 * @retval [4] Failure. <code>inet_pton()</code> failed.
 */
extern u8t unstdsocket_isvalidipaddr(const i8 *ip_address_arg);


/**
 * @brief Opens a TCP/IPv4 socket connection fd (file descriptor) to the specified destination.
 * @param destination_ip_address_arg Should be a pointer to a valid, null-terminated heap-allocated / c-array buffer
 *                                   containing the target IP address.
 * @param destination_port_number_arg A valid port number (u16t) within 1-65535
 *                                    containing the target Port number.
 * @returns A number (s32t) indicating the state of the operation.
 * @retval [>=3] A valid, open fd handle to the specified target.
 * @retval [3>x>=0] A valid, open fd handle to stdin=0, stdout=1, stderr=2 which is considered
 *              an abnormal response under normal circumstances.
 * @retval [-1] Insufficient parameter. `destination_ip_address_arg` is NULL. See `destination_ip_address_arg`.
 * @retval [-2] Insufficient parameter. `destination_ip_address_arg` is an empty string. See `destination_ip_address_arg`.
 * @retval [-3] Insufficient parameter. `destination_ip_address_arg` is not a valid IP address. See `destination_ip_address_arg`.
 * @retval [-4] Insufficient parameter. `destination_port_number_arg` is not a valid port number
 *              within the range of 1-65535. See `destination_port_number_arg`.
 * @retval [-5] Failure. <code>unstdsocket_isvalidipaddr()</code> failed.
 * @retval [-6] Failure. <code>socket()</code> failed.
 * @retval [-7] Failure. <code>connect()</code> failed.
 */
extern s32t unstdsocket_opentcpfd4(
        const i8 *destination_ip_address_arg,
        const u16t destination_port_number_arg
);

#endif //UNSTDLIB_UNSTDSOCKET_H
