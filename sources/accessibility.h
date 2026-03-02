#pragma once

#include <string>
#include <vector>

class AccessiblityTest {
public:
  AccessiblityTest(const AccessiblityTest &) = default;
  AccessiblityTest(AccessiblityTest &&) = delete;
  AccessiblityTest &operator=(const AccessiblityTest &) = default;
  AccessiblityTest &operator=(AccessiblityTest &&) = delete;
  virtual ~AccessiblityTest() = default;

  virtual bool check(std::string&, std::vector<std::string>);
};

class ConnectionTest : public AccessiblityTest {
	bool check(std::string& directory, std::vector<std::string> filenames) override;
};

class ResourceTest : public AccessiblityTest {
	bool check(std::string& address, std::vector<std::string> ports) override;
};