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
      make coverage_collect test-report
      mkdir $out
      mv coverage $out/
      mv coverage.info $out/
      mv test-suite/test-report.txt $out/
      cat $out/test-report.txt
    '';
});

in

{
  linux-x86 = doit "linux-x86" pkgsi686Linux.stdenv;
  linux-makecontext = doit "linux-makecontext" pkgsi686Linux.stdenv;
  posix = doit "posix" pkgsi686Linux.stdenv;
}
