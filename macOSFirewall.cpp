#include <iostream>
#include <cstdlib>
#include <vector>
#include <sstream>

// Function to run a shell command and capture output
std::string runCommand(const std::string& cmd) {
    std::string result;
    char buffer[128];
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "ERROR";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

// Function to block an IP address
void blockIP(const std::string& ip) {
    std::string command = "echo 'block drop out quick on en0 from any to " + ip + "' | sudo tee -a /etc/pf.conf > /dev/null";
    system(command.c_str());
}

// Function to apply firewall rules
void applyFirewallRules() {
    system("sudo pfctl -f /etc/pf.conf");
    system("sudo pfctl -E");
}

// Function to get all IPs of a domain
std::vector<std::string> resolveDomainIPs(const std::string& domain) {
    std::vector<std::string> ips;
    std::string output = runCommand("dig +short " + domain);
    std::istringstream stream(output);
    std::string ip;

    while (std::getline(stream, ip)) {
        ips.push_back(ip);
    }
    return ips;
}

int main() {
    std::string domain = "example.com";
    std::cout << "🔎 Resolving IPs for: " << domain << "\n";

    std::vector<std::string> blocklist = resolveDomainIPs(domain);

    if (blocklist.empty()) {
        std::cout << "❌ No IPs found for " << domain << "\n";
        return 1;
    }

    std::cout << "🚀 Blocking the following IPs:\n";
    for (const auto& ip : blocklist) {
        blockIP(ip);
        std::cout << "✅ Blocked: " << ip << std::endl;
    }

    applyFirewallRules();
    std::cout << "🔥 Firewall rules applied!\n";
    return 0;
}
