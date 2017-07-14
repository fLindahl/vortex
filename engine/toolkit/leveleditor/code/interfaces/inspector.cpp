#include "inspector.h"
#include "imgui.h"
#include "imgui_dock.h"
#include "../basetool.h"
#include "../toolhandler.h"
#include "application/game/entity.h"
#include "../selecttool.h"

namespace Interface
{
	__ImplementClass(Interface::Inspector, 'Ispc', Interface::InterfaceBase)

	Inspector::Inspector()
	{
		this->graphicsPropertyInspector = new LevelEditor::GraphicsPropertyInspector();
	}

	Inspector::~Inspector()
	{
	}

	void Inspector::Run()
	{


		if (ImGui::BeginDock("Inspector", NULL, ImGuiWindowFlags_NoSavedSettings))
		{
			const Ptr<Game::Entity>& selectedEntity = Tools::ToolHandler::Instance()->SelectTool()->GetSelectedEntity();

			if (selectedEntity != nullptr)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2,2));
				ImGui::Columns(1);
				ImGui::Separator();

				// Iterate dummy objects with dummy members (all the same data)
				for (uint i = 0; i < selectedEntity->GetNumProperties(); i++)
				{	
					Ptr<Game::BaseProperty> property = selectedEntity->Property(i);

					this->graphicsPropertyInspector->SetProperty(property);

					//use the object pointer as a base ID.
					size_t uid = (size_t)property.get_unsafe();
					ImGui::PushID(uid);

					// Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
					ImGui::AlignFirstTextHeightToWidgets();  
					bool node_open = ImGui::TreeNode("Object", "%s", this->graphicsPropertyInspector->GetName().AsCharPtr(), uid);
					ImGui::Columns(2);
					ImGui::NextColumn();
					if(node_open)
					{
						ImGui::PushID(i); // Use field index as identifier.
        				{
							this->graphicsPropertyInspector->DrawGUI();
						}
						ImGui::PopID();

						ImGui::TreePop();
					}
					ImGui::PopID();
					ImGui::Separator();
				}

				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::PopStyleVar();



				/*
				ImGui::Text("Transform:");
				ImGui::Text("%f | %f | %f | %f", selectedEntity->GetTransform().getrow(0).x(), selectedEntity->GetTransform().getrow(0).y(), selectedEntity->GetTransform().getrow(0).z(), selectedEntity->GetTransform().getrow(0).w());
				ImGui::Text("%f | %f | %f | %f", selectedEntity->GetTransform().getrow(1).x(), selectedEntity->GetTransform().getrow(1).y(), selectedEntity->GetTransform().getrow(1).z(), selectedEntity->GetTransform().getrow(1).w());
				ImGui::Text("%f | %f | %f | %f", selectedEntity->GetTransform().getrow(2).x(), selectedEntity->GetTransform().getrow(2).y(), selectedEntity->GetTransform().getrow(2).z(), selectedEntity->GetTransform().getrow(2).w());
				ImGui::Text("%f | %f | %f | %f", selectedEntity->GetTransform().getrow(3).x(), selectedEntity->GetTransform().getrow(3).y(), selectedEntity->GetTransform().getrow(3).z(), selectedEntity->GetTransform().getrow(3).w());

				ImGui::Text("Rotate:");

				float xRot = 0;	float yRot = 0;	float zRot = 0;

				ImGui::SliderFloat("X", &xRot, -0.05f, 0.05f, "%.3f", 1.0f);
				ImGui::SliderFloat("Y", &yRot, -0.05f, 0.05f, "%.3f", 1.0f);
				ImGui::SliderFloat("Z", &zRot, -0.05f, 0.05f, "%.3f", 1.0f);

				Math::mat4 transform = Math::mat4::multiply(Math::mat4::rotationyawpitchroll(yRot, xRot, zRot), selectedEntity->GetTransform());
				selectedEntity->SetTransform(transform);

				float yaw = atan2(-transform.getrow0().z(), transform.getrow0().x());
				float pitch = asin(transform.getrow0().y());
				float roll = atan2(-transform.getrow2().y(), transform.getrow1().y());

				ImGui::Text("Pitch : %f | Yaw : %f | Roll : %f", pitch, yaw, roll);
				ImGui::Separator();
				*/


				/*
				// CUBEMAPS --------------------------------------------
				Game::CubeMapEntity* cm = dynamic_cast<Game::CubeMapEntity*>(selectedEntity);

				if (cm != nullptr)
				{
					ImGui::SliderFloat("InnerRange", &cm->GetCubeMapNode()->InnerScale().x(), 0.00001f, 100.0f, "%.3f", 2.0f);
					ImGui::SliderFloat("OuterRange", &cm->GetCubeMapNode()->OuterScale().x(), 0.00001f, 100.0f, "%.3f", 2.0f);
				}

				Game::GeometryProxyEntity* gpe = dynamic_cast<Game::GeometryProxyEntity*>(selectedEntity);

				if (gpe != nullptr)
				{
					Math::vec4 scale = gpe->GetScale();

					ImGui::SliderFloat("Scale X", &scale.x(), 1.0f, 100.0f, "%.3f", 3.0f);
					ImGui::SliderFloat("Scale Y", &scale.y(), 1.0f, 100.0f, "%.3f", 3.0f);
					ImGui::SliderFloat("Scale Z", &scale.z(), 1.0f, 100.0f, "%.3f", 3.0f);

					gpe->SetScale(scale);

					if (ImGui::Button("Add CubeMap To Proxy", { 100, 40 }))
					{
						std::shared_ptr<Edit::AddEntity> command = std::make_shared<Edit::AddEntity>(Graphics::MainCamera::Instance()->GetPosition(), Render::ResourceServer::Instance()->LoadModel("resources/models/cubemap_icon.mdl"));
						commandManager->DoCommand(command);
						command->entity->SetGeometryProxy(gpe->GetGeometryProxy());
					}
				}

				//LIGHTS --------------------------------------------------

				Game::ModelEntitySpotLight* aa = dynamic_cast<Game::ModelEntitySpotLight*>(this->selectedEntity);
				Game::PointLightEntity* bb = dynamic_cast<Game::PointLightEntity*>(this->selectedEntity);

				if (aa != nullptr)
				{
					ImGui::Text("Spot Light");

					float angle = aa->GetSpotLightAngle();
					if (ImGui::SliderFloat("Angle", &angle, 1.0f, 90.0f, "%.1f"))
					{
						aa->SetSpotLightAngle(angle);
					}
					float length = aa->GetSpotLightLength();
					if (ImGui::SliderFloat("Length", &length, 1.0f, 50.0f, "%.1f"))
					{
						aa->SetSpotLightLength(length);
					}

					Math::vec4 color = aa->GetSpotLightColor();
					if (ImGui::SliderFloat("Red", &color.x(), 0.0f, 1.0f, "%.01f"))
					{
						aa->SetSpotLightColor(color);
					}
					if (ImGui::SliderFloat("Green", &color.y(), 0.0f, 1.0f, "%.01f"))
					{
						aa->SetSpotLightColor(color);
					}
					if (ImGui::SliderFloat("Blue", &color.z(), 0.0f, 1.0f, "%.01f"))
					{
						aa->SetSpotLightColor(color);
					}

				}
				if (bb != nullptr)
				{
					ImGui::Text("Point Light");

					float radius = bb->GetPointLightRadius();
					if (ImGui::SliderFloat("Radius", &radius, 1.0f, 50.0f, "%.1f"))
					{
						bb->SetPointLightRadius(radius);
					}

					Math::vec4 color = bb->GetPointLightColor();
					if (ImGui::SliderFloat("Red", &color.x(), 0.0f, 1.0f, "%.01f"))
					{
						bb->SetPointLightColor(color);
					}
					if (ImGui::SliderFloat("Green", &color.y(), 0.0f, 1.0f, "%.01f"))
					{
						bb->SetPointLightColor(color);
					}
					if (ImGui::SliderFloat("Blue", &color.z(), 0.0f, 1.0f, "%.01f"))
					{
						bb->SetPointLightColor(color);
					}
				}
				*/
			}
		}
		ImGui::EndDock();
	}
}


