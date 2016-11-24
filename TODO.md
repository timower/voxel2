
- UI:
	* One VBO for all UI? -> create quads in VBO.
	* Currently only text. -> use one texture atlas for all UI (buttons can be stretched?).
	* System: track all UI buttons and stuff -> if dirty update VBO and graphics component.

- Debug performance UI.

- Increase entity size and stuff.

- sub entities via entity component zero in entity or remove component zero?

- Terrain generation.

- File storage.

- Scripting language? (lisp, ...)

FIXES:

- Use generation & asserts.

- Investigate using multiple containers vs boolean flags. (one Handle array, multiple data arrays?)

- Use init message more.

- Messages vs simple function call?

- Collision resolution. (mainly when falling at high speed, prevent bounce)
