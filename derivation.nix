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
    rev = "f6cb71ee302b4eddd046e363e10b7414b14562d9";
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
