/*
**
** Copyright 2013, kod21236@gmail.com
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef STDOUT_RENDERER_H_
#define STDOUT_RENDERER_H_


namespace anatomy {

class StdoutRenderer {
public:
	StdoutRenderer(){}
	virtual ~StdoutRenderer(){}
	void render(void* buffer,int length);
private:
	char mBuffer[256];
};

}  // namespace anatomy

#endif  // STDOUT_RENDERER_H_

