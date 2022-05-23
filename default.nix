with (import <nixpkgs> {});
callPackage (
{ stdenv
, ecdsautils
, libuecc
, pkg-config
}:

stdenv.mkDerivation {
  name = "ecdsautil-node";

  buildInputs = [
    ecdsautils
    libuecc
  ];

  nativeBuildInputs = [
    pkg-config
  ];
}) {}

