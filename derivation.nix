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
    rev = "a2f51e7de27e516b8d415855fe00b55ae3e65da6";
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
