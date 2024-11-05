import "GameEngine" for Engine

Engine.Init()

var OnUpdate = Fn.new {
    System.print("Subscribtion is working!")
}

Engine.Subscribe(OnUpdate)