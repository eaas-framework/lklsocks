#ifndef SCOPEGUARD_H_
#define SCOPEGUARD_H_

#include <utility>

// courtesy of Andrei Alexandrescu
template<typename Fun>
class ScopeGuard {
    Fun f;
    bool active;
public:
    ScopeGuard(Fun f) :
            f(std::move(f)), active(true) {
    }
    ScopeGuard() = delete;
    ScopeGuard(const ScopeGuard &) = delete;
    ScopeGuard(ScopeGuard && rhs) :
        f(std::move(rhs.f)),
        active(rhs.active) {
        rhs.dismiss();
    }
    ~ScopeGuard() {
        if (this->active) {
            this->f();
        }
    }
    void dismiss() {
        this->active = false;
    }
    ScopeGuard &operator=(const ScopeGuard &) = delete;
};

template <typename Fun>
ScopeGuard<Fun> scopeGuard(Fun f) {
    return ScopeGuard<Fun>(std::move(f));
}

#endif /* SCOPEGUARD_H_ */
