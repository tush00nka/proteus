#pragma once

#include <string_view>
#include <vector>

class AccessiblityTest {
public:
	// AccessiblityTest() = default;
  	// AccessiblityTest(const AccessiblityTest &) = default;
  	// AccessiblityTest(AccessiblityTest &&) = delete;
  	// AccessiblityTest &operator=(const AccessiblityTest &) = default;
  	// AccessiblityTest &operator=(AccessiblityTest &&) = delete;
  	virtual ~AccessiblityTest() = default;

  	virtual bool check(std::string_view, std::vector<std::string>&) = 0;
};

class ConnectionTest : public AccessiblityTest {
public:
	ConnectionTest() = default;
	bool check(std::string_view address, std::vector<std::string>& ports) override;
};

class ResourceTest : public AccessiblityTest {
public:
	ResourceTest() = default;
	bool check(std::string_view path, std::vector<std::string>& filenames) override;
};