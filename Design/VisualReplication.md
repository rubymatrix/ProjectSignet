## Modular Visual Replication 

Reacting to equipment and option changes.

### What needs to be tracked?

- Race: When the player changes race, we need to change the root mesh and reload all equipment for the specific race
- Face
- Equipment Slots (Main, Sub, Ranged, Head, Body, Hands, Legs, Feet)


### Efficient Replciation

Everything we send across the wired needs to be boiled down to the smallest byte footprint we can comfortably rehydrate on the other side.

We can do that easily with a mix of uint8/uint16/uint32 values.

Race and Face are already uint8's because they're enumerations.

Equipment IDs are how we send those, ideally we represent this as a short, uint16.

Shorts have a limitation on max value obviously, but we will never hit that limit.

This is the structure we'll send as a package when something is updated.  

```cpp
USTRUCT(BlueprintType)
struct SIGNETGAME_API FVisualState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ERace Race;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EFace Face;

	UPROPERTY(VisibleAnywhere) uint16 MainID = 0;
	UPROPERTY(VisibleAnywhere) uint16 SubID = 0;
	UPROPERTY(VisibleAnywhere) uint16 RangeID = 0;
	UPROPERTY(VisibleAnywhere) uint16 HeadID = 0;
	UPROPERTY(VisibleAnywhere) uint16 BodyID = 0;
	UPROPERTY(VisibleAnywhere) uint16 HandsID = 0;
	UPROPERTY(VisibleAnywhere) uint16 LegsID = 0;
	UPROPERTY(VisibleAnywhere) uint16 FeetID = 0;
};
```

Why don't we send changes per-field?  Slow.  It's much faster to send a chunk of bytes once, than to send 10 chunks of bytes potentially.

If a player changes their race, we have to send the full structure anyways.

