<?xml version="1.0" encoding="utf-8" ?>

<scene>
    <image>
        <width> 800</width>
        <height>600</height>
    </image>
    <pinhole/>
    <diffuse id="meshmaterial">
        <emission>1</emission>
    </diffuse>
    <mesh>
        <material id="meshmaterial"/>
        <polygon>
            <point>-1 -1 -1.5</point>
            <point>-1  1 -1.5</point>
            <point> 1  1 -1.5</point>
        </polygon>
        <polygon>
            <point>-1 -1 -1.5</point>
            <point> 1 -1 -1.5</point>
            <point> 1  1 -1.5</point>
        </polygon>
    </mesh>
</scene>
