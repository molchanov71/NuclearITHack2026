package com.hexteam.app.discovery;

import org.springframework.stereotype.Component;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.InterfaceAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;

@Component
public class NetworkAddressResolver {
    /**
     * Выбирает пригодные LAN-адреса и broadcast-адреса для discovery.
     * Предпочтение отдаётся IPv4-интерфейсам, которые реально подняты в сети.
     */

    public List<InetAddress> broadcastAddresses() {
        return discoveryTargets().stream()
                .map(DiscoveryTarget::broadcastAddress)
                .distinct()
                .toList();
    }

    public InetAddress resolvePrimaryAddress() {
        for (NetworkInterface networkInterface : allInterfaces()) {
            if (!isUsable(networkInterface)) {
                continue;
            }
            for (InterfaceAddress interfaceAddress : networkInterface.getInterfaceAddresses()) {
                InetAddress address = interfaceAddress.getAddress();
                if (address instanceof Inet4Address inet4Address && !inet4Address.isLoopbackAddress()) {
                    return inet4Address;
                }
            }
        }
        return InetAddress.getLoopbackAddress();
    }

    public List<NetworkInterfaceSnapshot> interfaceSnapshots() {
        List<NetworkInterfaceSnapshot> snapshots = new ArrayList<>();
        for (NetworkInterface networkInterface : allInterfaces()) {
            if (!isUsable(networkInterface)) {
                continue;
            }
            List<String> addresses = new ArrayList<>();
            List<String> broadcasts = new ArrayList<>();
            for (InterfaceAddress interfaceAddress : networkInterface.getInterfaceAddresses()) {
                InetAddress address = interfaceAddress.getAddress();
                if (address instanceof Inet4Address inet4Address && !inet4Address.isLoopbackAddress()) {
                    addresses.add(inet4Address.getHostAddress());
                }
                InetAddress broadcast = interfaceAddress.getBroadcast();
                if (broadcast instanceof Inet4Address inet4Broadcast) {
                    broadcasts.add(inet4Broadcast.getHostAddress());
                }
            }
            if (!addresses.isEmpty() || !broadcasts.isEmpty()) {
                snapshots.add(new NetworkInterfaceSnapshot(
                        networkInterface.getName(),
                        networkInterface.getDisplayName(),
                        addresses,
                        broadcasts
                ));
            }
        }
        return snapshots;
    }

    public List<DiscoveryTarget> discoveryTargets() {
        List<DiscoveryTarget> targets = new ArrayList<>();
        for (NetworkInterface networkInterface : allInterfaces()) {
            if (!isUsable(networkInterface)) {
                continue;
            }
            for (InterfaceAddress interfaceAddress : networkInterface.getInterfaceAddresses()) {
                InetAddress address = interfaceAddress.getAddress();
                InetAddress broadcast = interfaceAddress.getBroadcast();
                if (address instanceof Inet4Address inet4Address
                        && !inet4Address.isLoopbackAddress()
                        && broadcast instanceof Inet4Address inet4Broadcast) {
                    targets.add(new DiscoveryTarget(inet4Address, inet4Broadcast));
                }
            }
        }
        return targets;
    }

    private List<NetworkInterface> allInterfaces() {
        try {
            Enumeration<NetworkInterface> interfaces = NetworkInterface.getNetworkInterfaces();
            List<NetworkInterface> result = new ArrayList<>();
            while (interfaces.hasMoreElements()) {
                result.add(interfaces.nextElement());
            }
            return result;
        } catch (SocketException exception) {
            throw new IllegalStateException("Не удалось получить список сетевых интерфейсов", exception);
        }
    }

    private boolean isUsable(NetworkInterface networkInterface) {
        try {
            return networkInterface.isUp()
                    && !networkInterface.isLoopback()
                    && !networkInterface.isVirtual();
        } catch (SocketException exception) {
            return false;
        }
    }
}
