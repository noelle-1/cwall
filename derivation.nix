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
    rev = "ca908f955b8fe815540bbd779a9ec848bb90a192";
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
