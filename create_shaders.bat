@echo off
REM Skript na vytvorenie struktury shader suborov pre island demo
REM Spustite v korennom priecinku projektu ppgso_projekt

echo Vytvaram strukturu shaderov...

REM Vytvorenie hlavneho priecinku pre shadery
if not exist "shader\island_demo" mkdir "shader\island_demo"

REM Vytvorenie hlavnych shaderov
echo. > "shader\island_demo\basic_vert.glsl"
echo. > "shader\island_demo\basic_frag.glsl"

echo. > "shader\island_demo\phong_vert.glsl"
echo. > "shader\island_demo\phong_frag.glsl"

echo. > "shader\island_demo\normal_map_vert.glsl"
echo. > "shader\island_demo\normal_map_frag.glsl"

echo. > "shader\island_demo\transparent_vert.glsl"
echo. > "shader\island_demo\transparent_frag.glsl"

echo. > "shader\island_demo\instanced_vert.glsl"
echo. > "shader\island_demo\instanced_frag.glsl"

echo. > "shader\island_demo\skybox_vert.glsl"
echo. > "shader\island_demo\skybox_frag.glsl"

echo. > "shader\island_demo\water_vert.glsl"
echo. > "shader\island_demo\water_frag.glsl"

echo. > "shader\island_demo\particle_vert.glsl"
echo. > "shader\island_demo\particle_frag.glsl"

echo. > "shader\island_demo\shadow_map_vert.glsl"
echo. > "shader\island_demo\shadow_map_frag.glsl"

REM Vytvorenie priecinku pre post-processing shadery
if not exist "shader\island_demo\post_process" mkdir "shader\island_demo\post_process"

echo. > "shader\island_demo\post_process\grayscale_frag.glsl"
echo. > "shader\island_demo\post_process\convolution_frag.glsl"
echo. > "shader\island_demo\post_process\bloom_frag.glsl"

REM Zdielany vertex shader pre post-processing
echo. > "shader\island_demo\post_process\post_process_vert.glsl"

echo.
echo ========================================
echo Shadery boli uspesne vytvorene!
echo ========================================
echo.
echo Vytvorene shadery:
echo   - basic (vertex + fragment)
echo   - phong (vertex + fragment)
echo   - normal_map (vertex + fragment)
echo   - transparent (vertex + fragment)
echo   - instanced (vertex + fragment)
echo   - skybox (vertex + fragment)
echo   - water (vertex + fragment)
echo   - particle (vertex + fragment)
echo   - shadow_map (vertex + fragment)
echo.
echo Post-processing shadery:
echo   - grayscale (fragment)
echo   - convolution (fragment)
echo   - bloom (fragment)
echo   - post_process (vertex - zdielany)
echo.
echo Lokacia: shader\island_demo\
echo.

pause