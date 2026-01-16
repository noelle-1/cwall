{
  stdenv,
  pkg-config,
  libnotify,
  gtk4,
  gnumake,
}:
stdenv.mkDerivation rec {
  name = "cwall-${version}";
  version = "1.0";

  src = builtins.fetchGit {
    url = "git@git.ivynet.org:noelle/cwall";
    rev = "f0c2dc498e84dbae7b8408f14f47fe45c916f95d";
  };

  nativeBuildInputs = [
    pkg-config
    libnotify
    gtk4
    gnumake
  ];

  buildInputs = [ ];
  hardeningDisable = [ "all" ];

    buildPhase = ''
        make
    '';

    installPhase = ''
        make install PREFIX=$out
    '';
}
