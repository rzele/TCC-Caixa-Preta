function [ ret ] = mediamovel( data, k)
%MEDIAMOVEL Summary of this function goes here
%   Detailed explanation goes here
    dataSize = length(data);
    ret = data(1:k);
    for x=k:dataSize
        ret(x) = sum(data(x-k+1:x))/k;
    end
end