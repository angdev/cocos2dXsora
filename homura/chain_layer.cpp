// Ŭnicode please
#include "stdafx.h"
#include "chain_layer.h"

USING_NS_CC;

ChainLayer::ChainLayer(GameWorld *world) 
    : world_(world),
    chain_tex_(nullptr) {
    
}

ChainLayer::~ChainLayer() {
    chain_dict_.clear();
    CC_SAFE_RELEASE_NULL(chain_tex_);
}

bool ChainLayer::init() {
    if(!CCLayer::init()) {
        return false;
    }

    //메시지 핸들러 등록
    RegisterMsgFunc(this, &ChainLayer::OnDestroyMessage);

    const char *chain_file = "bdsm_chain.png";
    chain_tex_ = CCTextureCache::sharedTextureCache()->addImage(chain_file);
    chain_tex_->retain();
    ccTexParams tex_param = {
        GL_LINEAR,
        GL_LINEAR,
        GL_REPEAT,
        GL_REPEAT
    };
    chain_tex_->setTexParameters(&tex_param);
    
    CCGLProgram *prog = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture);
    this->setShaderProgram(prog);

    return true;
}

void ChainLayer::draw() {
    if(chain_dict_.empty()) {
        return;
    }
    CC_NODE_DRAW_SETUP();

    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);
    ccGLBindTexture2D(chain_tex_->getName());

    struct Vertex {
        glm::vec2 pos;
        float s, t;
    };
    std::vector<Vertex> vert_list(4 * chain_dict_.size());
    std::vector<unsigned short> index_list(6 * chain_dict_.size());

    float tex_width = chain_tex_->getContentSize().width;
    float tex_height = chain_tex_->getContentSize().height;
    //적절한 체인의 크기처럼 보이게 수정하기
    const float scale = 0.15f;   
    tex_width *= scale;
    tex_height *= scale;

    int i = 0;
    for(auto iter : chain_dict_) {
        const ChainRenderState &state = iter.second;

        //텍스쳐의 변화는 마스터가 사슬은 떙기는 느낌이니까
        //기준점을 노예로 한다
        const glm::vec2 &target = state.master_pos;
        const glm::vec2 &start = state.slave_pos;
        SR_ASSERT(start != target);
        const glm::vec2 dir = glm::normalize(target - start);
        const glm::vec2 normal(-dir.y, dir.x);
        
        int base_idx = 4 * i;

        Vertex &left_start = vert_list[base_idx + 0];
        Vertex &right_start = vert_list[base_idx + 1];
        Vertex &right_target = vert_list[base_idx + 2];
        Vertex &left_target = vert_list[base_idx + 3];

        {
            glm::vec2 left = start + tex_width / 2 * normal;
            glm::vec2 right = start - tex_width / 2 * normal;
            left_start.pos = left;
            right_start.pos = right;
            left_start.s = 0;
            left_start.t = 0;
            right_start.s = 0;
            right_start.t = 1;
        }
        {
            glm::vec2 left = target + tex_width / 2 * normal;
            glm::vec2 right = target - tex_width / 2 * normal;

            glm::vec2 dir = target - start;
            float length = glm::length(dir);
            float texcoord_length = length / tex_height * (tex_height / tex_width);
            left_target.pos = left;
            right_target.pos = right;
            left_target.s = texcoord_length;
            left_target.t = 0;
            right_target.s = texcoord_length;
            right_target.t = 1;
        }
        i++;
    }

    for(int i = 0 ; i < chain_dict_.size() ; ++i) {
        int index_base_idx = 6 * i;
        int vert_base_idx = 4 * i;

        index_list[index_base_idx + 0] = vert_base_idx + 0;
        index_list[index_base_idx + 1] = vert_base_idx + 1;
        index_list[index_base_idx + 2] = vert_base_idx + 2;

        index_list[index_base_idx + 3] = vert_base_idx + 0;
        index_list[index_base_idx + 4] = vert_base_idx + 2;
        index_list[index_base_idx + 5] = vert_base_idx + 3;
    }

    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &vert_list[0].pos.x);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &vert_list[0].s);

    glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_SHORT, &index_list[0]);
    CC_INCREMENT_GL_DRAWS(1);
    
}

void ChainLayer::RequestDraw(int id, const glm::vec2 &slave_pos, const glm::vec2 &master_pos) {
    //dict에 있으면 정보 업데이트
    auto found = chain_dict_.find(id);
    if(found != chain_dict_.end()) {
        found->second.master_pos = master_pos;
        found->second.slave_pos = slave_pos;
        return;
    }

    ChainRenderState state;
    state.master_pos = master_pos;
    state.slave_pos = slave_pos;
    chain_dict_.insert(std::make_pair(id, state));
}

void ChainLayer::StopDraw(int id) {
    chain_dict_.erase(id);
}

void ChainLayer::OnDestroyMessage(DestroyMessage *msg) {
    StopDraw(msg->obj_id);
}
