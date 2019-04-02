package.path = package.path..";./?.lua"

local uuid = require "lua_uuid"

assert(type(uuid) == "table")

local function test_generator(gen)
  local first = gen()
  local second = gen()

  assert(first ~= second)
  assert(type(gen()) == "string")
end

test_generator(uuid)
test_generator(uuid.generate)
test_generator(uuid.generate_binary)

local hexuuid = "cf08a9b2-3492-4b71-9bd9-eb12d9f463a6"
local binuuid = "\207\008\169\178\052\146\075\113\155\217\235\018\217\244\099\166"

assert(assert(uuid.parse(hexuuid)) == binuuid)
assert(assert(uuid.unparse(binuuid)) == hexuuid)

assert(uuid.parse("foobar") == nil)
assert(uuid.unparse("foobar") == nil)
