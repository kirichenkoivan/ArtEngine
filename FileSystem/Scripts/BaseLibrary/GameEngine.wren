class Engine {
    
    static subscribers(value){__subscribers = value}

    static Init() {
        this.subscribers([])
        System.print("Scripts inited")
    }

    static GetSubscribers() {
        return __subscribers
    }

    static Subscribe(func) {
        if (func is Fn) {
            __subscribers.add(func)
        } else {
            System.print("ERROR: subscriber must be a function")
        }
    }

    static Update() {
        for (func in __subscribers) {
            func.call()
        }
    }
}