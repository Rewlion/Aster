from pathlib import Path
import os
import os.path

dxcExe = "../../../deps/dxc-1.6.2112/bin/dxc.exe"
outputFolder = "../../../game/aster/bin/shaders/spirv"

shadersToCompile = [
  {
    "file": "shaders/test.hlsl",
    "stages": {"vs", "ps"}
  }
]

Path(outputFolder).mkdir(parents=True, exist_ok=True)

shadersFolder = "shaders"

for shader in shadersToCompile:
  shaderFile = shader['file']
  if not os.path.isfile(shaderFile):
    print("error: {} does not exist.".format(shaderFile))
  if not shaderFile.endswith(".hlsl"):
    print("error: {} does not have .hlsl type.".format(shaderFile))

  for stage in shader['stages']:
    profile = "-T {}".format(stage + "_6_0")
    entry = "{}_main".format(stage)

    outputShaderFileName = "{}/{}_{}.spv".format(outputFolder, Path(shaderFile).stem, stage)

    cmd = """
    {exe} -spirv -HV 2018 {profile}
    -fspv-target-env=vulkan1.1
    -Fo {outputShader}
    -E {entry}
    {input}
    """.format(
      exe = dxcExe,
      profile = profile,
      outputShader = outputShaderFileName,
      entry = entry,
      input = shaderFile
    )

    cmd = cmd.replace("\n", "").replace("/","\\").strip()
    print(cmd)
    os.system(cmd)

