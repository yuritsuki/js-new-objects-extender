# JS RMG Object Extenders

This consumer plugin moves object behavior from the `h3era_plugins_js` fork into the current
`RMG_CustomizeObjectProperties` extender API. It registers the 20 object extenders that are not
already supplied by the current repository's core plugin or dedicated object packs:

- Altar of Mana, Ancient Lamp, Dream Teacher, Grave, Hermit's Shack, Hill Fort
- Jetsam, Junkman, Mineral Spring, Observatory, Prospector, Sea Barrel
- Seafaring Academy, Skeleton Transformer, Temple of Loyalty, Town Gate
- Trailblazer, Vial of Mana, Warlock's Lab, Watering Place

`BaseClasses.h` is a source compatibility layer for the per-object implementations from the JS fork. It
translates their map setup callback and helper methods into the current `extender::ObjectExtender`
virtual interface, while registration uses the versioned `RegisterObjectExtenderEx` API supplied by
`headers/EraPluginsAPI/ObjectExtenderAPI.hpp`.

The plugin also keeps the shared team-visit flags and water-object hooks used by these extenders.
Creature banks, WoG objects, Gazebo, Colosseum, Warehouses, Shrines, Spell Market, and University are
not registered here because the current repository already provides them in dedicated packs or in
`RMG_CustomizeObjectProperties`.

Build `Objects_JSRMGObjectsExtender.vcxproj` for Win32 with the repository's standard Visual
Studio configuration. Ensure this plugin and `RMG_CustomizeObjectProperties.era` are both installed
in the ERA plugins directory.
