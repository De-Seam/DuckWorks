-- Use GetTransform and SetTransform methods
local transform = Entity:GetTransform()

transform.position.x = transform.position.x + deltaTime * 110
Entity:SetTransform(transform)
