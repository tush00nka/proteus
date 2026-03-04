#pragma once

#include <string>
#include <vector>

class AccessiblityTest {
public:
	AccessiblityTest() = default;
  	AccessiblityTest(const AccessiblityTest &) = default;
  	AccessiblityTest(AccessiblityTest &&) = delete;
  	AccessiblityTest &operator=(const AccessiblityTest &) = default;
  	AccessiblityTest &operator=(AccessiblityTest &&) = delete;
  	virtual ~AccessiblityTest() = default;

  	virtual bool check(const std::string&, std::vector<std::string>&) = 0;
};

class ConnectionTest : public AccessiblityTest {
public:
	ConnectionTest() = default;
	bool check(const std::string& address, std::vector<std::string>& ports) override;
};

class ResourceTest : public AccessiblityTest {
public:
	ResourceTest() = default;
	bool check(const std::string& path, std::vector<std::string>& filenames) override;
};