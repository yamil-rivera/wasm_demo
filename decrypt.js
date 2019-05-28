{
    const ccallArrays = (func, returnType, paramTypes=[], params, {heapIn="HEAPF32", heapOut="HEAPF32", returnArraySize=1}={}) => {

      const heapMap = {}
      heapMap.HEAP8 = Int8Array // int8_t
      heapMap.HEAPU8 = Uint8Array // uint8_t
      heapMap.HEAP16 = Int16Array // int16_t
      heapMap.HEAPU16 = Uint16Array // uint16_t
      heapMap.HEAP32 = Int32Array // int32_t
      heapMap.HEAPU32 = Uint32Array // uint32_t
      heapMap.HEAPF32 = Float32Array // float
      heapMap.HEAPF64 = Float64Array // double

      let res
      let error
      const returnTypeParam = returnType=="array" ? "number" : returnType
      const parameters = []
      const parameterTypes = []
      const bufs = []

      try {
          if (params) {
              for (let p=0; p<params.length; p++) {

                  if (paramTypes[p] == "array" || Array.isArray(params[p])) {

                      const typedArray = new heapMap[heapIn](params[p].length)

                      for (let i=0; i<params[p].length; i++) {
                          typedArray[i] = params[p][i]
                      }

                      const buf = Module._malloc(typedArray.length * typedArray.BYTES_PER_ELEMENT)

                      switch (heapIn) {
                          case "HEAP8": case "HEAPU8":
                              Module[heapIn].set(typedArray, buf)
                              break
                          case "HEAP16": case "HEAPU16":
                              Module[heapIn].set(typedArray, buf >> 1)
                              break
                          case "HEAP32": case "HEAPU32": case "HEAPF32":
                              Module[heapIn].set(typedArray, buf >> 2)
                              break
                          case "HEAPF64":
                              Module[heapIn].set(typedArray, buf >> 3)
                              break
                      }

                      bufs.push(buf)
                      parameters.push(buf)
                      parameters.push(params[p].length)
                      parameterTypes.push("number")
                      parameterTypes.push("number")

                  } else {
                      parameters.push(params[p])
                      parameterTypes.push(paramTypes[p]==undefined ? "number" : paramTypes[p])
                  }
              }
          }

          res = Module.ccall(func, returnTypeParam, parameterTypes, parameters)
      } catch (e) {
          error = e
      } finally {
          for (let b=0; b<bufs.length; b++) {
              Module._free(bufs[b])
          }
      }

      if (error) throw error

      if (returnType=="array") {
          const returnData = []

          for (let v=0; v<returnArraySize; v++) {
              returnData.push(Module[heapOut][res/heapMap[heapOut].BYTES_PER_ELEMENT+v])
          }

          return returnData
      } else {
          return res
      }
    }
    // Wrap around cwrap also, as a bonus
    const cwrapArrays = (func, returnType, paramTypes, {heapIn="HEAPF32", heapOut="HEAPF32", returnArraySize=1}={}) => {
      return params => ccallArrays(func, returnType, paramTypes, params, {heapIn, heapOut, returnArraySize})
    }

    const inputBuffer = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
    const HexKey = "F3E07650A34F212807B7E49707DF1A1E7506929AFEFE79A3418C5FC47FB899D4"
    const HexIV = "3C453D7847740658EEAA9A1F94800F77"

    let JSkey = []
    let JSiv = []

    for (let i=0; i<HexKey.length; i+=2) {
      JSkey[i/2] = parseInt(HexKey[i].concat(HexKey[i+1]), 16);
    }

    for (let i=0; i<HexIV.length; i+=2) {
      JSiv[i/2] = parseInt(HexIV[i].concat(HexIV[i+1]), 16);
    }

    console.log(inputBuffer);
    console.log(JSkey);
    console.log(JSiv);

    let result = ccallArrays("AES_CBC_decrypt_buffer", "array", ["number", "number", "number", "number"], [JSkey, JSiv, inputBuffer, inputBuffer.length], {heapIn: "HEAPU8", heapOut: "HEAPU8", returnArraySize: inputBuffer.length});
    console.log(result);
}

