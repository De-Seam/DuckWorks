local position = Entity:GetPosition()

position.mX = position.mX + deltaTime * 110
Entity:SetPosition(position)