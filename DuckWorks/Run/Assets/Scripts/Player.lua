local transform = Entity:GetTransform()

transform.mPosition.mX = transform.mPosition.mX + deltaTime * 110
Entity:SetTransform(transform)

print(tostring(transform.mPosition.mX))