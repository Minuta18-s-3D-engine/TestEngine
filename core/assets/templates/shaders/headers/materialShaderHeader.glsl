void loadCurrentMaterial() {
    uint base = u_CurrentMaterialStartId;
    {{ unpack_lines }}   
}

void main() {
    loadCurrentMaterial();

    {{ user_func }}();
}