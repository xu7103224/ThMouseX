local function CharacterUpdate(___body: object): void
    local position = ___body.position
    Position.X = position.X
    Position.Y = position.Y
end

return {
    --    Target method,     prehook,   posthook
    {'THMHJ.Character:Update', nil, CharacterUpdate},
}