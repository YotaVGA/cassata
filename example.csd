<?xml version="1.0" encoding="utf-8" ?>

<scene>
    <image>
        <width> 800</width>
        <height>600</height>
    </image>
    <pinhole/>
    <diffuse id="lightmaterial">
        <emissivity>1</emissivity>
    </diffuse>
    <diffuse id="meshmaterial">
        <reflectivity>0.8</reflectivity>
    </diffuse>
    <mesh>
        <material id="lightmaterial"/>
        <!--back wall-->
        <polygon>
            <point>-0.5  1 -2  </point>
            <point>-0.5  1 -1.5</point>
            <point> 0.5  1 -1.5</point>
        </polygon>
        <polygon>
            <point>-0.5  1 -2  </point>
            <point> 0.5  1 -2  </point>
            <point> 0.5  1 -1.5</point>
        </polygon>
    </mesh>
    <mesh>
        <material id="meshmaterial"/>
        <polygon>
            <point>-1 -1 -2.5</point>
            <point>-1  1 -2.5</point>
            <point> 1  1 -2.5</point>
        </polygon>
        <polygon>
            <point>-1 -1 -2.5</point>
            <point> 1 -1 -2.5</point>
            <point> 1  1 -2.5</point>
        </polygon>
    </mesh>
</scene>
