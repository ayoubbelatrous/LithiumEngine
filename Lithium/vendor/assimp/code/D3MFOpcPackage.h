/*
Open Asset Import Library (assimp)
----------------------------------------------------------------------

Copyright (c) 2006-2018, assimp team


All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------
*/

#ifndef D3MFOPCPACKAGE_H
#define D3MFOPCPACKAGE_H

#include <memory>

#include <assimp/IOSystem.hpp>
#include <assimp/irrXMLWrapper.h>

namespace Assimp {
namespace D3MF {

using XmlReader = irr::io::IrrXMLReader ;
using XmlReaderPtr = std::shared_ptr<XmlReader> ;

struct OpcPackageRelationship {
    std::string id;
    std::string type;
    std::string target;
};

class D3MFZipArchive;

class D3MFOpcPackage {
public:
    D3MFOpcPackage( IOSystem* pIOHandler, const std::string& rFile );
    ~D3MFOpcPackage();
    IOStream* RootStream() const;
    bool validate();
    static bool isZipArchive( IOSystem* pIOHandler, const std::string& rFile );

protected:
    std::string ReadPackageRootRelationship(IOStream* stream);

private:
    IOStream* mRootStream;
    std::unique_ptr<D3MFZipArchive> mZipArchive;
};

} // Namespace D3MF
} // Namespace Assimp

#endif // D3MFOPCPACKAGE_H
