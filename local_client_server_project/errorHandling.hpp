#pragma once

#include <stdexcept>
#include <string>

class CommandException : public std::runtime_error {
public:
    explicit CommandException(const std::string& msg) : std::runtime_error(msg) {}
};

class UserException : public CommandException {
public:
    explicit UserException(const std::string& msg) : CommandException(msg) {}
};

class AuthException : public CommandException {
public:
    explicit AuthException(const std::string& msg) : CommandException(msg) {}
};

class FileException : public CommandException {
public:
    explicit FileException(const std::string& msg) : CommandException(msg) {}
};

class InvalidCommandException : public CommandException {
public:
    explicit InvalidCommandException(const std::string& msg) : CommandException(msg) {}
};

class AccessDeniedException : public CommandException {
public:
    explicit AccessDeniedException(const std::string& msg) : CommandException(msg) {}
};

class UncategorisedError : public CommandException {
public:
    explicit UncategorisedError(const std::string& msg) : CommandException(msg) {}
};