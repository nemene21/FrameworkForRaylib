#include <health_component.hpp>

// <Health Component>
HealthComponent::HealthComponent(Entity *entity, float max_hp):
    Component(ComponentType::HEALTH, entity),
    hp {max_hp},
    max_hp {max_hp},
    dead {false} {}

void HealthComponent::draw_gui_info() {
    if (ImGui::CollapsingHeader(("Health##" + std::to_string(id)).c_str())) {
        ImGui::Indent(25.f);

        ImGui::Text("Hp: ");
        ImGui::SameLine();
        ImGui::Text((std::to_string(hp) + "/" + std::to_string(max_hp)).c_str());
        
        ImVec4 col = dead ? ImVec4{1, 0, 0, 1} : ImVec4{0, 1, 0, 1};
        ImGui::Text("Died: ");
        ImGui::SameLine();
        ImGui::TextColored(col, dead ? "true" : "false");
        ImGui::Unindent(25.f);
    }
}


void HealthComponent::hurt(float damage) {
    hp -= damage;

    hurt_signal.emit(entity);
    check_death();
}

void HealthComponent::heal(float add) {
    hp += add;

    healed_signal.emit(entity);
}

bool HealthComponent::check_death() {
    if (dead)
        return dead;
    
    dead = hp <= 0;

    if (dead)
        died_signal.emit(entity);
    
    return dead;
}