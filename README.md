### Prerequisites

- sudo chmod +w /etc/pf.conf  # Ensure pf.conf is writable
- sudo pfctl -e  # Enable Packet Filter (PF) firewall

### Running the Program

- make macOSFirewall
- sudo ./macOSFirewall

<pre>
curl http://example.com --connect-timeout 10

MacBook-Air-2:macOS-firewall anish$ curl http://example.com --connect-timeout 10
curl: (28) Failed to connect to example.com port 80 after 9851 ms: Operation timed out
</pre>

### To Unblock an IP

- sudo nano /etc/pf.conf and delete the line with the specific rule
- sudo pfctl -f /etc/pf.conf # Reload Firewall Rules

### Disable IPv6

- On interface en0: networksetup -setv6off Wi-Fi
- Verify IPv6 disabled: curl -6 http://example.com --connect-timeout 10

### Test Firewall (Manually)

- dig +short example.com
- Add rules for all IP's listed in /etc/pf.conf
- Example rule: block drop out quick on en0 from any to [IP]
- Reload all rules: sudo pfctl -f /etc/pf.conf
- Check rules added: sudo pfctl -sr
- Test Requests timeout: curl http://example.com --connect-timeout 10
