#pragma once

#include <memory>

// Copy / move enable

#define COPY_ENABLED(Type) \
    Type(const Type& other); \
    Type& operator=(const Type& other);

#define MOVE_ENABLED(Type) \
    Type(Type&& other) noexcept; \
    Type& operator=(Type&& other) noexcept;

#define COPY_MOVE_ENABLED(Type)\
    COPY_ENABLED(Type) \
    MOVE_ENABLED(Type)

// Copy / move disable

#define COPY_DISABLED(Type) \
    Type(const Type& other) = delete; \
    Type& operator=(const Type& other) = delete;

#define MOVE_DISABLED(Type) \
    Type(Type&& other) noexcept = delete; \
    Type& operator=(Type&& other) noexcept = delete;

#define COPY_MOVE_DISABLED(Type)\
    COPY_DISABLED(Type) \
    MOVE_DISABLED(Type)

// Compilation firewalls

#define COMPILATION_FIREWALL(Type) \
    struct Private; \
    std::unique_ptr<Private> m_pPrivate; \
public: \
    virtual ~Type() noexcept;

#define COMPILATION_FIREWALL_COPY(Type) \
    COMPILATION_FIREWALL(Type) \
    COPY_ENABLED(Type)

#define COMPILATION_FIREWALL_MOVE(Type) \
    COMPILATION_FIREWALL(Type) \
    MOVE_ENABLED(Type)

#define COMPILATION_FIREWALL_COPY_MOVE(Type) \
    COMPILATION_FIREWALL(Type) \
    COPY_ENABLED(Type) \
    MOVE_ENABLED(Type)

// Getters and setters

#define DECLARE_GETTER_MUTABLE(Name, ReturnType) \
    ReturnType& Name();

#define DEFINE_GETTER_MUTABLE(Class, Name, ReturnType, Object) \
    ReturnType& Class::Name() { \
        return Object; \
    }

#define DECLARE_GETTER_IMMUTABLE(Name, ReturnType) \
    const ReturnType& Name() const;

#define DEFINE_GETTER_IMMUTABLE(Class, Name, ReturnType, Object) \
    const ReturnType& Class::Name() const { \
        return Object; \
    }

#define DECLARE_GETTER_IMMUTABLE_COPY(Name, ReturnType) \
    ReturnType Name() const;

#define DEFINE_GETTER_IMMUTABLE_COPY(Class, Name, ReturnType, Object) \
    ReturnType Class::Name() const { \
        return Object; \
    }

#define DECLARE_GETTER_CONST_CORRECT(Name, ReturnType) \
    DECLARE_GETTER_MUTABLE(Name, ReturnType) \
    DECLARE_GETTER_IMMUTABLE(Name, ReturnType)

#define DEFINE_GETTER_CONST_CORRECT(Class, Name, ReturnType, Object) \
    DEFINE_GETTER_MUTABLE(Class, Name, ReturnType, Object) \
    DEFINE_GETTER_IMMUTABLE(Class, Name, ReturnType, Object)

#define DECLARE_SETTER_CONSTREF(Name, Type) \
    void Name(const Type& value);

#define DEFINE_SETTER_CONSTREF(Class, Name, Object) \
    void Class::Name(const decltype(Object)& value) { \
        Object = value; \
    }

#define DEFINE_SETTER_CONSTREF_EXPLICIT(Class, Name, Type, Object) \
    void Class::Name(const Type& value) { \
        Object = value; \
    }

#define DECLARE_SETTER_COPY(Name, Type) \
    void Name(Type value);

#define DEFINE_SETTER_COPY(Class, Name, Object) \
    void Class::Name(decltype(Object) value) { \
        Object = value; \
    }

#define DEFINE_SETTER_COPY_EXPLICIT(Class, Name, Type, Object) \
    void Class::Name(Type value) { \
        Object = value; \
    }