-- script.lua

sumNumbers = function(a,b)
	   printMessage("You can still call C++ functions from Lua functions!")
	   return a + b
end

local entityManager = EntityManager.new()

--entityManager.create_entity();

--game.addEnemy(17, "James")