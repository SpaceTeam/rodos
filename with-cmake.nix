with import <nixpkgs> {};

let doit = (target: env: env.mkDerivation {
  name = "rodos-${target}";
  src = ./.;
  buildInputs = [cmake lcov];
  cmakeFlags = [
    "-DCMAKE_TOOLCHAIN_FILE=../cmake/port/${target}.cmake"
    "-DEXECUTABLE=ON"
    "-DCOVERAGE=ON"
  ];
  installPhase =
    ''
      make coverage
      mkdir $out
      mv coverage $out/
      mv coverage.info $out/
    '';
});

in

  map (cfg: doit (builtins.elemAt cfg 0) (builtins.elemAt cfg 1))
  [
  ["linux-x86" pkgsi686Linux.stdenv]
  ]
