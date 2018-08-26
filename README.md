# EnumPerfTestProject
Simple performance testing commandlet for StaticEnum&lt;&gt;() vs FindObject() in UE4.

## Setup
1. Apply my engine changes (available here: https://github.com/nedwardsnae/UnrealEngine)
2. Generate project files
3. Compile this project (and the engine)
4. Run the project with the additional command line arguments:
`-run=EnumPerfTest`

## Example output
Running on an i7 3770k @ 4.5GHz, compiled in Development

```
[FindObject]      Total: 0.0164144933, Avg: 0.0000016414
[FindObjectExact] Total: 0.0294141024, Avg: 0.0000029414
[StaticEnum]      Total: 0.0012433007, Avg: 0.0000001243
[StaticEnumNaive] Total: 0.0020990893, Avg: 0.0000002099

[Raw]      Total: 0.0004395805, Avg: 0.0000000440
[Old]      Total: 0.1629130915, Avg: 0.0000162913
[New]      Total: 0.1464482434, Avg: 0.0000146448
[NewNaive] Total: 0.1443553306, Avg: 0.0000144355

0: One
1: Two
2: Three
3: MAX
```
